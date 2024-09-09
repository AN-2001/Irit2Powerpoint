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
        public ITDParser.ImportSettings ImportSettings;
        public GlRenderer.RenderSettings RenderSettings;
    };

    struct LoadResult
    {
        public ITDParser.ITDMesh Mesh;
        public GlRenderer.RenderSettings RenderSettings;
    };

    public class GlResourceManager
    {
        private const int TASK_COUNT = 64;
        private Dictionary<string, GlResource> ResourceMap;
        private Dictionary<string, LoadResult> ResultMap;
        private Task[] Tasks;
        private Queue<LoadRequest> LoadQueue;
        private Mutex Mutex;

        public GlResourceManager()
        {
            Mutex = new Mutex();
            ResourceMap = new Dictionary<string, GlResource>();
            ResultMap = new Dictionary<string, LoadResult>();
            Tasks = new Task[TASK_COUNT];
            LoadQueue = new Queue<LoadRequest>();
        }

        private bool QueueImpl(LoadRequest Request)
        {
            int i;

            /* Exit if we already loaded this key. */
            if (ResourceMap.ContainsKey(Request.Path) || ResultMap.ContainsKey(Request.Path))
                return true;

            for (i = 0; i < TASK_COUNT; i++) {
                if (Tasks[i] == null || Tasks[i].IsCompleted)
                {
                    Tasks[i] = Task.Factory.StartNew(
                        () =>
                        {
                            LoadResult
                                Result = new LoadResult();

                            try
                            {
                                Result.RenderSettings = Request.RenderSettings;
                                Result.Mesh = ITDParser.Parse(Request.Path, Request.ImportSettings);
                            } catch (ParseException)
                            {
                                return;
                            }

                            Mutex.WaitOne();
                            ResultMap[Request.Path] = Result;
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
                /* Never delete the grid. */
                if (Pair.Key == Constants.GRID_KEY)
                    continue;

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
            LoadResult Result;

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

            Result = ResultMap[Key];
            ResultMap.Remove(Key);
            Mutex.ReleaseMutex();

            Ret = new GlResource(Result.Mesh, Result.RenderSettings);
            ResourceMap[Key] = Ret;
            return Ret;
        }


        public void SetResource(string Key, ITDParser.ITDMesh Mesh)
        {
            GlResource
                Resource = new GlResource(Mesh);

            TryLoadFromQueue();
            ResourceMap[Key] = Resource;
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
