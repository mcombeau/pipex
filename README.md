# pipex

<p align="center">
  <img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/pipexe.png" alt="Pipex 42 project badge"/>
</p>

pipex is a 42 school project aimed to better understand shell redirection and pipes, by handling them in C.

This pipex program takes an input file, performs a command on it, pipes the result to another command which then writes its result to an output file. The result is virtually identical to this kind of shell command:
```
$ < input_file command1 | command2 > output file
```

---

:us: Read my articles in English about the concepts tackled in this project: 

* [Creating and Killing Child Processes in C](https://www.codequoi.com/en/creating-and-killing-child-processes-in-c/)
* [Pipe: an Inter-Process Communication Method](https://www.codequoi.com/en/pipe-an-inter-process-communication-method/)

:fr: Lire mes articles en français sur les concepts abordés dans ce projet :

* [Créer et tuer des processus fils en C](https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/)
* [Pipe : une méthode de communication inter-processus](https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/)

---

## Status
Finished 01/05/2022. Grade: 118% (here_doc bonus failed: prints an extra prompt line after LIMITER is given as input.)

## Usage
To compile, use ```make```, ```make all``` or ```make bonus```.

### Regular pipex execution
Pipex can handle 2 or more commands. To execute:

```
$ ./pipex input_file command1 command2 ... commandn output_file
```
The output will be written to the specified output file. If the output file exists, it will be overwritten. This replicates the shell command:
```
$ < input_file command1 | command2 | ... | commandn > output_file
```

Each command can be given arguments and options if surrounded by quotation marks. For example:
```
$ ./pipex example.txt "cat" "grep x" "sed s/l/.../g" outfile.txt
```

### Pipex heredoc execution
You can also run pipex with a heredoc, like this:
```
$ ./pipex here_doc LIMITER command1 command2 ... commandn output_file
```
You will then be asked to input text which will be used as input for the commands. In this case, the specified output file will not be overwritten but appended, which replicates the behavior of the shell command:
```
$ << LIMITER command1 | command2 | ... | commandn >> output_file
```

---
Made by mcombeau: mcombeau@student.42.fr | LinkedIn: [mcombeau](https://www.linkedin.com/in/mia-combeau-86653420b/) | Website: [codequoi.com](https://www.codequoi.com)
