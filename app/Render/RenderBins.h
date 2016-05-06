

//Lovingly taken from openmw
enum RenderBins
{
	RenderBin_Sky = -1,
	RenderBin_Default = 0, // osg::StateSet::OPAQUE_BIN
	RenderBin_Water = 9,
	RenderBin_DepthSorted = 10, // osg::StateSet::TRANSPARENT_BIN
	RenderBin_OcclusionQuery = 11,
	RenderBin_FirstPerson = 12,
	RenderBin_SunGlare = 13,
	RenderBin_GUI = 14
};