//  Copyright (c) 2014 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import "METileFactory.h"
#import "MEMapInfo.h"
#import "MEProtocols.h"

/** A conveniience class to assist with the creation of map info objects.*/
@interface MEMapFactory : NSObject

/**Creates an virtual map info object for an internet map.*/
+(MEVirtualMapInfo*) createInternetMapInfo:(id<METileLoader>) meTileLoader
								   mapName:(NSString*) mapName
							   urlTemplate:(NSString*) urlTemplate
								subDomains:(NSString*) subDomains
								  maxLevel:(unsigned int) maxLevel
									zOrder:(unsigned int) zOrder
							   workerCount:(int) workerCount
						   useNetworkCache:(BOOL) useNetworkCache
							 diskCacheFile:(NSString*) diskCacheFile;

/**Creates an virtual map info object for an Altus internet map that covers the poles.*/
+(MEVirtualMapInfo*) createInternetMapInfo:(id<METileLoader>) meTileLoader
								   mapName:(NSString*) mapName
						 urlTemplateCenter:(NSString*) urlTemplateCenter
						  urlTemplateNorth:(NSString*) urlTemplateNorth
						  urlTemplateSouth:(NSString*) urlTemplateSouth
								subDomains:(NSString*) subDomains
								  maxLevel:(unsigned int) maxLevel
									zOrder:(unsigned int) zOrder
							   workerCount:(int) workerCount
						   useNetworkCache:(BOOL) useNetworkCache
							 diskCacheFile:(NSString*) diskCacheFile
						  compressTextures:(BOOL) compressTextures;

/**Creates an virtual map info object for an Altus internet map that covers the poles.*/
+(MEVirtualMapInfo*) createInternetTerrainMapInfo:(id<METileLoader>) meTileLoader
										  mapName:(NSString*) mapName
								urlTemplateCenter:(NSString*) urlTemplateCenter
								 urlTemplateNorth:(NSString*) urlTemplateNorth
								 urlTemplateSouth:(NSString*) urlTemplateSouth
									   subDomains:(NSString*) subDomains
										 maxLevel:(unsigned int) maxLevel
										   zOrder:(unsigned int) zOrder
									  workerCount:(int) workerCount
								  useNetworkCache:(BOOL) useNetworkCache
									diskCacheFile:(NSString*) diskCacheFile;

/**Creates an virtual map info object for a packaged raster map.*/
+(MEVirtualMapInfo*) createRasterPackageMapInfo:(id<METileLoader>) meTileLoader
										mapName:(NSString*) mapName
								packageFileName:(NSString*) packageFileName
										 zOrder:(unsigned int) zOrder
									workerCount:(int) workerCount;
@end
