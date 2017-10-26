.import "region.js" as MyRegion;

var ar_regions = {};

function deleteRegionsAndLayouts()
{
    for (var prop in ar_regions)
    {
        ar_regions[prop].destroyRegion();
        gc();
        ar_regions[prop].destroy(1000);
    }
}

function setRegions(regions_list)
{
    for (var prop in regions_list)
    {
        ar_regions[regions_list[prop][0]] = MyRegion;
        ar_regions[regions_list[prop][0]].createRegion(regions_list[prop]);
    }
}


function startShowMedia(media)
{
    if ( Object.keys(ar_regions).length > 0) // prevent to call function of nonexisting regions
    {
         ar_regions[media.region_name].startShowMedia(media);
    }
}

function stopShowMedia(media)
{
    if ( Object.keys(ar_regions).length > 0) // prevent to call function of nonexisting regions
    {
        ar_regions[media.region_name].stopShowMedia(media);
    }
}


