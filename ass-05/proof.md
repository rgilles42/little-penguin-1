The misc device driver works well as intended.  
After loading:
```sh
rgilles [ ~/little-penguin-1/ass-05 ]$ cat /dev/fortytwo 
rgillesrgilles [ ~/little-penguin-1/ass-05 ]$ echo -n rgilles | tee /dev/fortytwo > /dev/null
rgilles [ ~/little-penguin-1/ass-05 ]$ echo -n zsfejlflekfjedfsdfjkwfjweklfjkw | tee /dev/fortytwo > /dev/null
tee: /dev/fortytwo: Invalid argument
```
