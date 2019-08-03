//  Copyright (c) 2014 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import "METileWorker.h"
#import "METerrainTile.h"

/** Downloads map tiles.*/
@interface METileDownloader : METileWorker

/** Initialize instance for use with spherical mercator raster maps on the internet.
 @param urlTemplate Template for downloads. For example:
 If you have multiple subdomains you would use a url string like this:
 http://{s}.tiles.mapbox.com/v3/mymap/{z}/{x}/{y}.jpg
 {s} is not required, but if you do use it, make sure the subDomains parameter
 contains a comma delimited list of subdomains, i.e.: a,b,c,d
 @param subDomains Comma separate list of subdomains
 */
-(id) initWithURLTemplate:(NSString*) urlTemplate
			   subDomains:(NSString*) subDomains
		  useNetworkCache:(BOOL) useNetworkCache;

/** Initialize instance for use with AltusRaster global raster tiles on the internet.*/
-(id) initWithURLTemplateCenter:(NSString*) urlTemplateCenter
			   urlTemplateNorth:(NSString*) urlTemplateNorth
			   urlTemplateSouth:(NSString*) urlTemplateSouth
					 subDomains:(NSString*) subDomains
				useNetworkCache:(BOOL) useNetworkCache;

/**URL template string for tiles within the web mercator range of latitudes.*/
@property (retain) NSString* urlTemplateCenter;
/**URL template string for tiles north of the web mercator latitude that conform to the Altus projection.*/
@property (retain) NSString* urlTemplateNorth;
/**URL template string for tiles south of the web mercator latitude that conform to the Altus projection.*/
@property (retain) NSString* urlTemplateSouth;
/**An array or sub-domain strings to round-robin requests for tiles. For example: {"a","b","c","d"} would indicate to round-robin request tiles from  a.mytiles.com, b.mytiles.com, c.mytiles.com, and d.mytiles.com for a url template "{s}.mytiles.com".*/
@property (retain) NSArray* subDomains;
/**Whether or not to use the NSURL cace.*/
@property (assign) BOOL useNetworkCache;
/**Current subdomain index.*/
@property (assign) int currentSubdomain;
/**Time in seconds to stop waiting for a tile to download.*/
@property (assign) int timeOutInterval;
/**Last http response code.*/
@property (assign) int httpResponse;
@end

/** Downloads terrain tiles. Handles conversion of gray-scale png compressed terrain tiles.*/
@interface METerrainTileDownloader : METileDownloader
@property (nonatomic, retain) METerrainTile* terrainTile;
@end

