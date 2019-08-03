//  Copyright (c) 2014 BA3, LLC. All rights reserved.
#pragma once
#import <Foundation/Foundation.h>
#import "METileProvider.h"
#import "METerrainTile.h"
#import "METileWorker.h"
#import "MEPackage.h"
#import "MEProtocols.h"
#import "MEMapView.h"

/** Serves as a tile provider that manages TileWorker objects. This demonstrates a farming out work on a per-tile basis and only doing the work that is necessary to serve up data that is in view.*/
@interface METileFactory : METileProvider
/**The map name this factory is providing tiles for.*/
@property (retain) NSString* mapName;
/**The name of the map view.*/
@property (retain) NSString* mapViewName;
/**Current outstanding tile request count.*/
@property (retain) NSMutableArray* activeTileRequests;
/**Array of METileWorker objects.*/
@property (retain) NSMutableArray* tileWorkers;
/**The full path of a file to use as a cache for downloaded tiles. The file will be created if it does not exist. The file will be a sqlite file that that conforms to the Altus Map Package schema.*/
@property (retain) NSString* diskCacheFile;
/**Instance Altus Map Package cache object downloaded tiles will be written to.*/
@property (retain) MEPackage* packageCache;
/**The number of cache hits.*/
@property (assign) unsigned long foundInPackageCount;
/**The number or cache misses.*/
@property (assign) unsigned long notFoundInPackageCount;
/**The number of tiles that were downloaded, but then no needed because the map location was changed.*/
@property (assign) unsigned long notNeededCount;
/**The number of tiles written to the package.*/
@property (readonly) unsigned long writtenToPackageCount;
/**The number of times a package write had to be retried because another thread had the database locked.*/
@property (readonly) unsigned long writtenToPackageRetryCount;
/**The number of times a package write completely failed.*/
@property (readonly) unsigned long writtenToPackageFailureCount;
/**Whether or not to display verbose messages.*/
@property (assign) BOOL verbose;

/**The queue on which the worker works. Default is DISPATCH_QUEUE_PRIORITY_DEFAULT.
 You can set this to:
 DISPATCH_QUEUE_PRIORITY_HIGH
 DISPATCH_QUEUE_PRIORITY_DEFAULT
 DISPATCH_QUEUE_PRIORITY_LOW
 DISPATCH_QUEUE_PRIORITY_BACKGROUND*/
@property (assign) dispatch_queue_priority_t targetQueuePriority;


-(void) initPackageCache:(NSString*) packageFileName
			 packageType:(MEPackageType) packageType;

-(void) addWorker:(METileWorker*) tileWorker;

-(void) finishTile:(METileProviderRequest *) meTileRequest
		 newCacheData:(NSData*) newCacheData;

-(void) finishTerrainTile:(METileProviderRequest *) meTileRequest
			  terrainTile:(METerrainTile*) terrainTile
				newCacheData:(NSData*) newCacheData;

/**Creates a factory that manages TileDownloader objects for serving up internet map tiles.*/
+(METileFactory*) createInternetTileFactory:(id<METileLoader>) meTileLoader
								urlTemplate:(NSString*) urlTemplate
								 subDomains:(NSString*) subDomains
								workerCount:(int) workerCount
							useNetworkCache:(BOOL) useNetworkCache
							  diskCacheFile:(NSString*) diskCacheFile
									mapName:(NSString*) mapName;

/**Creates a factory that manages TileDownloader objects for serving up Altus internet map tiles which cover the poles.*/
+(METileFactory*) createInternetTileFactory:(id<METileLoader>) meTileLoader
						  urlTemplateCenter:(NSString*) urlTemplateCenter
						   urlTemplateNorth:(NSString*) urlTemplateNorth
						   urlTemplateSouth:(NSString*) urlTemplateSouth
								 subDomains:(NSString*) subDomains
								workerCount:(int) workerCount
							useNetworkCache:(BOOL) useNetworkCache
							  diskCacheFile:(NSString*) diskCacheFile
									mapName:(NSString*) mapName;

/**Creates a factory that manages TileDownloader objects for serving up Altus internet terrain map tiles which cover the poles.*/
+(METileFactory*) createInternetTerrainTileFactory:(id<METileLoader>) meTileLoader
								 urlTemplateCenter:(NSString*) urlTemplateCenter
								  urlTemplateNorth:(NSString*) urlTemplateNorth
								  urlTemplateSouth:(NSString*) urlTemplateSouth
										subDomains:(NSString*) subDomains
									   workerCount:(int) workerCount
								   useNetworkCache:(BOOL) useNetworkCache
									 diskCacheFile:(NSString*) diskCacheFile
										   mapName:(NSString*) mapName;

/**Creates a factory that manages TilePackageReader objects for serving up a package map.*/
+(METileFactory*) createPackageTileFactory:(id<METileLoader>) meTileLoader
						   packageFileName:(NSString*) packageFileName
							   workerCount:(int) workerCount
								   mapName:(NSString*) mapName;

@end