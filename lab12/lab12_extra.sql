.read lab12.sql

CREATE TABLE smallest_int_having AS
  SELECT time, smallest FROM students GROUP BY smallest HAVING COUNT(*) = 1 ORDER BY smallest;

CREATE TABLE fa19favpets AS
  SELECT pet, COUNT(*) AS counts FROM students GROUP BY pet ORDER BY counts DESC LIMIT 10;


CREATE TABLE fa19dog AS
  SELECT pet, COUNT(*) AS counts FROM students GROUP BY pet HAVING pet = 'dog';


CREATE TABLE obedienceimages AS
  SELECT seven, instructor, COUNT(*) AS counts FROM students WHERE seven = '7' GROUP BY instructor;
