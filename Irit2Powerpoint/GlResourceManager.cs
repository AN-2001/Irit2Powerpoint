using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using System.Linq;
using System.Windows.Forms;

namespace Irit2Powerpoint
{

    public struct LoadRequest
    {
        public string Path;
        public string ImportSettings;
    };

    public class GlResourceManager
    {
        private const int TASK_COUNT = 64;
        private Dictionary<string, GlResource> ResourceMap;
        private Dictionary<string, ITDParser.ITDMesh> ResultMap;
        private Task[] Tasks;
        private Queue<LoadRequest> LoadQueue;
        private Mutex Mutex;

        public GlResourceManager()
        {
            Mutex = new Mutex();
            ResourceMap = new Dictionary<string, GlResource>();
            ResultMap = new Dictionary<string, ITDParser.ITDMesh>();
            Tasks = new Task[TASK_COUNT];
            LoadQueue = new Queue<LoadRequest>();
        }

        public string BuildResourceKey(string Path, string ImportSettings)
        {
            return Path + ":" + ImportSettings;
        }

        private bool QueueImpl(LoadRequest Request)
        {
            int i;
            string Key;

            Key = BuildResourceKey(Request.Path, Request.ImportSettings);

            /* Exit if we already loaded this key. */
            if (ResourceMap.ContainsKey(Key) || ResultMap.ContainsKey(Key))
                return true;

            for (i = 0; i < TASK_COUNT; i++) {
                if (Tasks[i] == null || Tasks[i].IsCompleted)
                {
                    Tasks[i] = Task.Factory.StartNew(
                        () =>
                        {
                            ITDParser.ITDMesh Mesh;
                            try
                            {
                                Mesh = ITDParser.Parse(Request.Path, Request.ImportSettings);
                            } catch (ParseException)
                            {
                                return;
                            }

                            Mutex.WaitOne();
                            ResultMap[Key] = Mesh;
                            Mutex.ReleaseMutex();
                        });
                    return true;
                }
            }

            return false;
        }

        /* Queue a load task on the threadpool. */
        public void QueueLoadFromDisk(LoadRequest Request)
        {
            /* First try to load already queued resources. */
            TryLoadFromQueue();
            /* Then try to queue the given key. */
            if (!QueueImpl(Request))
                LoadQueue.Enqueue(Request);
        }

        private void TryLoadFromQueue()
        {
            LoadRequest Request;
            Queue<LoadRequest> Copy = new Queue<LoadRequest>();

            while (LoadQueue.Count > 0)
            {
                Request = LoadQueue.Dequeue();
                if (!QueueImpl(Request))
                    Copy.Enqueue(Request);
            }

            Copy.ToList().ForEach(e => LoadQueue.Enqueue(e));
        }

        public void ConsistencyCleanup(string[] InUse)
        {
            TryLoadFromQueue();
            List<string> ToDelete = new List<string>();

            /* Iterate over all keys, destroy each one that's not in use. */
            foreach (KeyValuePair<string, GlResource> Pair in ResourceMap)
            {
                if (!InUse.Contains(Pair.Key))
                {
                    Pair.Value.Destroy();
                    ToDelete.Add(Pair.Key);
                }
            }
            ToDelete.ForEach(e => ResourceMap.Remove(e));
        }

        /* Return a resource from the resource map. */
        public GlResource GetResource(string Key)
        {
            GlResource Ret;
            ITDParser.ITDMesh Mesh;

            TryLoadFromQueue();
            if (ResourceMap.ContainsKey(Key))
                return ResourceMap[Key];

            /* If it doesn't exist then we're either stil loading or we 
             * need to finalize. */
            Mutex.WaitOne();
            if (!ResultMap.ContainsKey(Key))
            {
                Mutex.ReleaseMutex();
                throw new StillLoadingException();
            }

            Mesh = ResultMap[Key];
            ResultMap.Remove(Key);
            Mutex.ReleaseMutex();

            Ret = new GlResource(Mesh);
            ResourceMap[Key] = Ret;
            return Ret;
        }

        public void Destroy()
        {
            /* Join all threads. */
            Task[] Filtered = Tasks.Where(t=>t != null).ToArray();
            Task.WaitAll(Filtered);
            foreach (KeyValuePair<string, GlResource> Pair in ResourceMap)
                Pair.Value.Destroy();
        }
    }
}
