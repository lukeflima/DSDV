
# DSDV   

## Execution  
To make.  

```console
$ make
```
Make and execute.  
```console
$ make execute
```

## Test file  
First line contain number of nodes (n).  
Next n line should contain 3 atributes, the position of each node (x and y) followed by the ratio of reach.  

After this, one line containing the number of messages to be sent (m).  
Next m lines containing the source node and destination node ([1-n] folowing the order of the file) and the mensage do be sent.  
### Exemple:  
```
8
2 -8 6
1 -2 9
1 0 8
2 -2 10
1 -10 4
2 -8 4
3 -4 10
2 -8 6
3
1 5 Hello :D
1 2 Tudo okay
2 5 Vai dar certo

```
