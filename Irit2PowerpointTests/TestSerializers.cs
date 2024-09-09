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
        public void TestImportSettings()
        {
            ITDParser.ImportSettings Settings = new ITDParser.ImportSettings();
            ITDParser.ImportSettings After;

            Settings.PolygonFineness = 1;
            Settings.PolylineFineness = 2;
            Settings.IsolinesSamples = 3;
            Settings.PolygonOptimal = 4;
            Settings.PolylineOptimal = 5;

            After = ITDParser.DeserializeImportSettings(ITDParser.SerializeImportSettings(Settings));
            Assert.AreEqual(Settings, After);
        }

        [TestMethod]
        public void TestRenderSettings()
        {
            GlRenderer.RenderSettings Settings = GlRenderer.DefaultRenderSettings;
            GlRenderer.RenderSettings After;

            Settings.AntiAliasingLevel = 1;
            Settings.BackfaceCulling = true;
            Settings.BackgroundColour = new OpenTK.Vector3(4, 5, 6);
            Settings.DefaultCurveColour = new OpenTK.Vector3(7, 8, 9);
            Settings.DefaultSolidColour = new OpenTK.Vector3(10, 11, 12);
            Settings.DisplayGrid = false;
            Settings.DisplayNormals = true;
            Settings.FarPlane = 13;
            Settings.NearPlane = 14;
            Settings.OverrideColour = false;
            Settings.NormalLength = 15;
            Settings.GridColour = new OpenTK.Vector3(16, 17, 18);

            After = GlRenderer.DeserializeRenderSettings(GlRenderer.SerializeRenderSettings(Settings));
            Assert.AreEqual(Settings, After);
        }

    }
}
