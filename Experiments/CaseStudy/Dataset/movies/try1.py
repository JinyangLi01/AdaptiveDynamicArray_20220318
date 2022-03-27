import pymysql
import string
import random

connection = pymysql.connect(host='localhost',
                     # port=3306,
                     user='root',
                     passwd='ljy19980228',
                     db='end2end',
                     charset='utf8')


sql = "INSERT INTO `awards`(`director_name`, `ceremony`, `year`, `category`, `outcome`, `original_language`) \
    VALUES (%s, %s, %s, %s, %s, %s)"
T = []


def string_generator(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))



with connection:
    with connection.cursor() as cursor:
        # Create a new record
        sql = "INSERT INTO `awards`(`director_name`, `ceremony`, `year`, `category`, `outcome`, `original_language`) \
            VALUES (%s, %s, %s, %s, %s, %s)"
        for i in range(1, 11):
            c1 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            c2 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            c3 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            c4 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            c5 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            c6 = ''.join(random.sample(string.ascii_letters + string.digits, random.randint(1, 30)))
            cursor.execute(sql, (c1, c2, c3, c4, c5, c6))

    # connection is not autocommit by default. So you must commit to save
    # your changes.
    connection.commit()

    with connection.cursor() as cursor:
        # Read a single record
        sql = "SELECT * FROM `awards`"
        cursor.execute(sql)
        result = cursor.fetchall()
        print(result)

