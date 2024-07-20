using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Graphics;

namespace Irit2Powerpoint
{
    class GlResourceManager
    {
        private Dictionary<string, GlResource> ResourceMap;

        public GlResourceManager()
        {
            ResourceMap = new Dictionary<string, GlResource>();
        }

        public GlResource GetResource(string Filepath)
        {
            if (ResourceMap.ContainsKey(Filepath))
                return ResourceMap[Filepath];
            ResourceMap[Filepath] = new GlResource(Filepath);
            return ResourceMap[Filepath];
        }

        public void Destroy()
        {
            foreach (KeyValuePair<string, GlResource> Pair in ResourceMap)
                Pair.Value.Destroy();
        }
    }
}
