//
//  METerrainTile.h
//  AltusMappingEngine
//
//  Created by Bruce Shankle on 10/2/14.
//  Copyright (c) 2014 BA3, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface METerrainTile : NSObject

@property (nonatomic, assign) unsigned int width;
@property (nonatomic, assign) unsigned int height;
@property (nonatomic, retain) NSData* grayScalePngData;

-(id) initWithGrayScalePngData:(NSData*) grayScalePngData;

-(id) initWithWidth:(unsigned int) width andHeight:(unsigned int) height;

-(void) setHeightValue:(short int) height
					 x:(unsigned int) x
					 y:(unsigned int) y
			   isWater:(BOOL) isWater;


@end
