using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using System.Linq;
using System.Windows.Forms;

namespace Irit2Powerpoint
{
    public class GlResourceManager
    {
        private const int TASK_COUNT = 64;
        private Dictionary<string, GlResource> ResourceMap;
        private Dictionary<string, ITDParser.ITDMesh> MeshMap;
        private Task[] Tasks;
        private Queue<string> LoadQueue;
        private Mutex Mutex;
        private static SynchronizationContext SyncContext;

        public GlResourceManager()
        {
            Mutex = new Mutex();
            ResourceMap = new Dictionary<string, GlResource>();
            MeshMap = new Dictionary<string, ITDParser.ITDMesh>();
            Tasks = new Task[TASK_COUNT];
            LoadQueue = new Queue<string>();
            SyncContext = SynchronizationContext.Current;
        }

        private bool QueueImpl(string Key)
        {
            int i;

            /* Exit if we already loaded this key. */
            if (ResourceMap.ContainsKey(Key) || MeshMap.ContainsKey(Key))
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
                                Mesh = ITDParser.Parse(Key);
                            } catch (ParseException Ex)
                            {
                                return;
                            }

                            Mutex.WaitOne();
                            MeshMap[Key] = Mesh;
                            Mutex.ReleaseMutex();
                        });
                    return true;
                }
            }

            return false;
        }

        /* Queue a load task on the threadpool. */
        public void QueueLoadFromDisk(string Key)
        {
            /* First try to load already queued resources. */
            TryLoadFromQueue();
            /* Then try to queue the given key. */
            if (!QueueImpl(Key))
                LoadQueue.Enqueue(Key);
        }

        private void TryLoadFromQueue()
        {
            string Key;
            Queue<string> Copy = new Queue<string>();

            while (LoadQueue.Count > 0)
            {
                Key = LoadQueue.Dequeue();
                if (!QueueImpl(Key))
                    Copy.Enqueue(Key);
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
            ITDParser.ITDMesh Mesh;

            TryLoadFromQueue();
            if (ResourceMap.ContainsKey(Key))
                return ResourceMap[Key];

            /* If it doesn't exist then we're either stil loading or we 
             * need to finalize. */
            Mutex.WaitOne();
            if (!MeshMap.ContainsKey(Key))
            {
                Mutex.ReleaseMutex();
                throw new StillLoadingException();
            }

            Mesh = MeshMap[Key];
            MeshMap.Remove(Key);
            Mutex.ReleaseMutex();

            Ret = new GlResource(Mesh);
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
