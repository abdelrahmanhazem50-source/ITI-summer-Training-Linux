#!/bin/bash

read -r -s -p "Enter password: " password
echo

score=0
length_status="Missing"
uppercase_status="Missing"
lowercase_status="Missing"
digit_status="Missing"
special_status="Missing"

if [ "${#password}" -ge 8 ]; then
    length_status="OK"
    score=$((score + 1))
fi

if [[ "$password" =~ [A-Z] ]]; then
    uppercase_status="OK"
    score=$((score + 1))
fi

if [[ "$password" =~ [a-z] ]]; then
    lowercase_status="OK"
    score=$((score + 1))
fi

if [[ "$password" =~ [0-9] ]]; then
    digit_status="OK"
    score=$((score + 1))
fi

if [[ "$password" =~ [^a-zA-Z0-9] ]]; then
    special_status="OK"
    score=$((score + 1))
fi

echo
echo "Password Strength Report"
echo
echo "Length          : $length_status"
echo "Uppercase       : $uppercase_status"
echo "Lowercase       : $lowercase_status"
echo "Digit           : $digit_status"
echo "Special Char    : $special_status"
echo

if [ "$score" -eq 5 ]; then
    echo "Overall: Strong"
else
    echo "Overall: Weak"
fi
