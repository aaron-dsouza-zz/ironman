//
//  AppDelegate.h
//  Ironman3
//
//  Created by Aaron D'Souza on 14/06/19.
//  Copyright © 2019 Aaron D'Souza. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (readonly, strong) NSPersistentContainer *persistentContainer;

- (void)saveContext;


@end

