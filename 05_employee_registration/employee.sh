#!/bin/bash

echo "======================================"
echo "     Employee Registration System"
echo "======================================"

read -r -p "Enter Employee Name: " employee_name
read -r -p "Enter Age: " age
read -r -p "Enter Department: " department
read -r -p "Enter Salary: " salary

echo
echo "--------------------------------------"
echo "       Registered Employee Info"
echo "--------------------------------------"
echo "Name       : $employee_name"
echo "Age        : $age"
echo "Department : $department"
echo "Salary     : $salary"
echo "--------------------------------------"
