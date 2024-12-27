#!/usr/bin/expect -f

set password "1"

spawn ssh kleinera@10.10.0.2 mkdir -p /usr/local/bin
expect "password:"
send "$password\r"
expect eof

spawn scp src/cat/s21_cat src/grep/s21_grep kleinera@10.10.0.2:~/
expect "password:"
send "$password\r"
expect eof

spawn ssh kleinera@10.10.0.2
expect "password:"
send "$password\r"
expect "$ "
send "sudo mv ~/s21_cat /usr/local/bin\r"
expect "password:"
send "$password\r"
expect "$ "
send  "sudo mv ~/s21_grep /usr/local/bin\r"
expect eof

spawn ssh kleinera@10.10.0.2 ls -lah /usr/local/bin
expect "password:"
send "$password\r"
expect eof
