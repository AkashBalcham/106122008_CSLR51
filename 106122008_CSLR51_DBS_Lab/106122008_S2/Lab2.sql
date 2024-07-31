-- Q1) Creating table -- 
CREATE TABLE students IF NOT EXISTS(
    std_rollno INT PRIMARY KEY AUTO_INCREMENT, 
    std_name VARCHAR(100) NOT NULL, 
    std_dept VARCHAR(50),
    std_course1 CHAR(30),
    std_course2 CHAR(30),
    std_course3 CHAR(30),
)


-- INSERTING -- 
INSERT INTO students VALUES 
(1, 'Carlos', 'CSE', 'OS', 'FLAT', 'ECO'),
(2, 'Max', 'MEC', 'MATH', 'CN', 'DT'),
(3, 'Lewis', 'EEE', 'ENG', 'VR', 'ECO'),
(4, 'Charles', 'ICE', 'OS', 'DSA', 'ECS'),
(5, 'Lando', 'ECE', 'DSA', 'FLAT', 'MATH');


-- Delete course2 and course3 --
ALTER TABLE students DROP std_course2;
ALTER TABLE students DROP std_course3;

-- Add dob and email columns --
ALTER TABLE student ADD dob DATE NOT NULL,
email VARCHAR(100) NOT NULL check (email LIKE "%@nitt.edu");

-- Change data type of course1
ALTER TABLE student MODIFY std_course1 VARCHAR2(50);

-- update col name from rollno to rno --
ALTER TABLE student RENAME COLUMN std_rollno TO std_rno;

-- Change dbms to os --
UPDATE student SET std_course1 = "OS" WHERE std_course1 = "DBMS";

-- Delete students with name = "s__"
DELETE FROM student WHERE std_name LIKE "S%";

-- Show everyone boprn after 2005
SELECT * FROM student WHERE YEAR(dob) > 2005;


-- Drop table
DROP TABLE student;

-- Truncate
TRUNCATE TABLE student;

--Rename table
ALTER TABLE student RENAME TO clg_students;

--Comment
ALTER TABLE students COMMENT = 'This table contains information about college students';
