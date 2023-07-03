The debugfs works well as intended.  
After loading:
```sh
rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/id 
rgillesrgilles [ ~/little-penguin-1/ass-07 ]$ echo -n "rgilles" | tee /sys/kernel/debug/fortytwo/id > /dev/null
rgilles [ ~/little-penguin-1/ass-07 ]$ echo -n "rgilless" | tee /sys/kernel/debug/fortytwo/id > /dev/null
tee: /sys/kernel/debug/fortytwo/id: Invalid argument
rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/jiffies 
4322394170rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/jiffies 
4322395379rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/jiffies 
4322396263rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/jiffies 
4322397131rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/foo
rgilles [ ~/little-penguin-1/ass-07 ]$ echo "rgilless" | sudo tee /sys/kernel/debug/fortytwo/foo > /dev/null
rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/foo
rgilless
rgilles [ ~/little-penguin-1/ass-07 ]$ echo "rgilles" | sudo tee /sys/kernel/debug/fortytwo/foo > /dev/null
rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/foo
rgilles
rgilles [ ~/little-penguin-1/ass-07 ]$ sudo rmmod fortytwo
rgilles [ ~/little-penguin-1/ass-07 ]$ cat /sys/kernel/debug/fortytwo/foo
cat: /sys/kernel/debug/fortytwo/foo: No such file or directory
```

The locking mechanism with kernel mutexes allow for the elimination of race conditions.  
```sh
> for i in {1..1000}; do echo $i | sudo tee /sys/kernel/debug/fortytwo/foo >/dev/null; cat /sys/kernel/debug/fortytwo/foo; done
1
2
3
...
1000
```
No numbers are skipped or repeated twice, even though this for-loop should have a high entropy in terms of order of execution.
