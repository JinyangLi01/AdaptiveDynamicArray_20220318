show databases;
use end2end;
drop table awards;
CREATE TABLE awards (
	id int primary key,
    director_name VARCHAR(100),
    ceremony VARCHAR(100),
    year VARCHAR(100),
    category VARCHAR(100),
    outcome VARCHAR(100),
    original_language VARCHAR(100)
);

select * from awards;


SHOW VARIABLES LIKE "secure_file_priv";


LOAD DATA LOCAL INFILE '220k_awards_by_directors.csv' 
INTO TABLE awards 
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;

select * from awards;


show global variables like 'local_infile';

set global local_infile=true;
show global variables like 'local_infile';

LOAD DATA LOCAL INFILE '220k_awards_by_directors.csv' 
INTO TABLE awards 
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n'
IGNORE 1 ROWS;



show tables;
show databases;




drop table users;



