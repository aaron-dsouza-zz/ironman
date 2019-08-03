//
//  DBManager.h
//  Ironman2
//
//  Created by Aaron D'Souza on 13/06/19.
//  Copyright © 2019 Aaron D'Souza. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <sqlite3.h>

NS_ASSUME_NONNULL_BEGIN

@interface DBManager : NSObject

    @property (nonatomic, strong) NSString *documentsDirectory;
    @property (nonatomic, strong) NSString *databaseFilename;
    @property (nonatomic, strong) NSMutableArray *arrColumnNames;
    @property (nonatomic, strong) NSMutableArray *arrResults;
    @property (nonatomic) int affectedRows;
    
    @property (nonatomic) long long lastInsertedRowID;
    -(instancetype)initWithDatabaseFilename:(NSString *)dbFilename;
    -(void)copyDatabaseIntoDocumentsDirectory;
    -(void)runQuery:(const char *)query isQueryExecutable:(BOOL)queryExecutable;
    -(NSArray *)loadDataFromDB:(NSString *)query;
    -(void)executeQuery:(NSString *)query;
@end

NS_ASSUME_NONNULL_END
