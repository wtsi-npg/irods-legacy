--- Run these SQL statements, using the postgresql client psql,
--- if your ICAT DBMS is postgres.

--- New type and classes for Database Resources (DBR)
insert into R_TOKN_MAIN values ('resc_type',405,'database','','','','','1288631300','1288631300');

insert into R_TOKN_MAIN values ('resc_class',504,'postgresql','','','','','1288631300','1288631300');
insert into R_TOKN_MAIN values ('resc_class',505,'mysql','','','','','1288631300','1288631300');
insert into R_TOKN_MAIN values ('resc_class',506,'oracle','','','','','1288631300','1288631300');

--- New data_type for Database Objects (used with DBRs)
insert into R_TOKN_MAIN values ('data_type',1695,'database object','text','','','','1288631300','1288631300');

--- New table to hold specific-queries (SQL-based queries)
create table R_SPECIFIC_QUERY
(
   sql varchar(2700),
   create_ts varchar(32)
);

--- Add a couple built-in specific queries (to see the specific queries).
insert into R_SPECIFIC_QUERY (sql, create_ts) values ('select sql from r_specific_query', '01292940000');
insert into R_SPECIFIC_QUERY (sql, create_ts) values ('select sql from r_specific_query where sql like ?', '01292940000');

create unique index idx_specific_query on R_SPECIFIC_QUERY (sql);
