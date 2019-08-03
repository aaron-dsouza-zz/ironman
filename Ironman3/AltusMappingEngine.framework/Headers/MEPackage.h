//  Copyright (c) 2014 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "MEEnumerations.h"
#import "METileProvider.h"

/**Utility class for accessing Altus map packages.*/
@interface MEPackage : NSObject

/**Initialize a package for reading only.
 @param fileName Name of Altus map package sqlite file.
 */
-(id) initWithFileName:(NSString*) fileName;

/**Intialize a package for reading and writing.*/
-(id) initWithFileName:(NSString*) fileName
		andPackageType:(MEPackageType) packageType;

/**Filename of the package.*/
@property (readonly, retain) NSString* fileName;

/**Package type.*/
@property (readonly) MEPackageType packageType;

/**West-most boundary of map data.*/
@property (readonly) double minX;

/**Southern-most boundary of map data.*/
@property (readonly) double minY;

/**East-most boundary of map data.*/
@property (readonly) double maxX;

/**Northern-most boundary of map data.*/
@property (readonly) double maxY;

/**The minimimum tile level in the map data.*/
@property (readonly) unsigned int minLevel;

/**The maximum tile level in the map data.*/
@property (readonly) unsigned int maxLevel;

/**YES if the package can be read / written by more than one thread.*/
@property (readonly) BOOL isThreadSafe;

/**The number of times addTile has failed.*/
@property (readonly) unsigned long writtenToPackageFailureCount;

/**The number of times addTile has had to retry writing to the package because the underlying sqlite file was locked by another thread doing a write operation.*/
@property (readonly) unsigned long writtenToPackageRetryCount;

/**The number of times addTile has succeeded.*/
@property (readonly) unsigned long writtenToPackageCount;

/**Yes if you want to see debug messages in the console.*/
@property (assign) BOOL verbose;

/**Yes if the package is read-only (for example was included in the app bundle).*/
@property (readonly) BOOL isReadOnly;

/**How many times to attempt to write to packages if a write operation fails. Defaults to 5.*/
@property (assign) unsigned int maxPackageWriteRetries;

/**How long to sleep when a package write fails before retrying. Defaults to 2.0*/
@property (assign) double packageWriteRetrySleepInterval;

/**Return a tile from the package.
 @param tileId The unique 64-bit tile identifier for the tile.*/
-(NSData*) getTileUsingId:(uint64_t) tileId;

/**
 Inserts or updates the binary data for the specified tile in the package.
 NOTE: Adding a tile opens, writes, and closes a connection to the package's sqlite database. This work is performed on a background serial queue. If you have multiple instances of MEPackage using the same package file, it is possible for the write operation to fail because another connection may have a writer lock. In that case, the operation is tried again after a sleep interval (packageWriteRetrySleepInterval). This is repeated until the write is successful up to a maximum number of retries (maxPackageWriteRetries).
 @param uid The unique 64-bit ID for the tile. You can use the uid property of an METile object that you've created to specify a tile uid.
 @param tileData Binary data to be stored for this tile
 */
-(void) addTile:(uint64_t) uid
	   tileData:(NSData*) tileData;

-(NSArray*) getParents:(uint64_t) tileId;

-(CGRect) getBoundsUsingId:(uint64_t) tileId;

@end
