using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using Irit2Powerpoint;
using OpenTK;

namespace Irit2PowerpointTests
{
    [TestClass]
    public class TestSerializers
    {

        [TestMethod]
        public void TestRenderSettings()
        {
            GlRenderer.RenderSettings Settings = GlRenderer.DefaultRenderSettings;
            GlRenderer.RenderSettings After;

            Settings.LightPosition = new OpenTK.Vector3(1, 2, 3);
            Settings.LightColour = new OpenTK.Vector3(4, 5, 6);
            Settings.BackgroundColour = new OpenTK.Vector3(7, 8, 9);
            Settings.DefaultCurveColour = new OpenTK.Vector3(10, 11, 12);
            Settings.DefaultSolidColour = new OpenTK.Vector3(13, 14, 15);

            After = GlRenderer.DeserializeRenderSettings(GlRenderer.SerializeRenderSettings(Settings));
            Assert.AreEqual(Settings, After);
        }

    }
}
