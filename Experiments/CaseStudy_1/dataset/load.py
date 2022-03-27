# There are 225675 rows in awards dataseet

import string
import random
import pymysql.cursors
import pandas as pd

data = pd.read_csv(r"220k_with_id.csv")
data = data.where((pd.notnull(data)), None)

print(data[:4])


# Connect to the database
connection = pymysql.connect(host='localhost',
                             user='root',
                             password='LjyDq1998',
                             database='ADA1',
                             charset='utf8',
                             cursorclass=pymysql.cursors.DictCursor)

# create cursor
cursor=connection.cursor()

# creating column list for insertion
cols = "`,`".join([str(i) for i in data.columns.tolist()])

# Insert DataFrame recrds one by one.
for i,row in data.iterrows():
    sql = "INSERT INTO `awards6` (`" +cols + "`) VALUES (" + "%s,"*(len(row)-1) + "%s)"
    
    # print(sql)
    # print(tuple(row))

    cursor.execute(sql, tuple(row))

    # the connection is not autocommitted by default, so we must commit to save our changes
    connection.commit()
    # if i == 1000:
    #     break



# Execute query
sql = "SELECT * FROM `awards6`"
cursor.execute(sql)

# Fetch all the records
result = cursor.fetchall()
num = 1
for i in result:
    print(i)
    if num == 10:
        break
    num += 1

connection.close()


