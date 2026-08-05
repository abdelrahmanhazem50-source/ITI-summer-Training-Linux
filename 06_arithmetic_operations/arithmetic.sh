#!/bin/bash

read -r -p "Enter first number: " first_number
read -r -p "Enter second number: " second_number

sum=$((first_number + second_number))
difference=$((first_number - second_number))
multiplication=$((first_number * second_number))

echo
echo "Sum = $sum"
echo "Difference = $difference"
echo "Multiplication = $multiplication"

if [ "$second_number" -eq 0 ]; then
    echo "Division = Cannot divide by zero"
    echo "Modulus = Cannot divide by zero"
else
    division=$((first_number / second_number))
    modulus=$((first_number % second_number))
    echo "Division = $division"
    echo "Modulus = $modulus"
fi
