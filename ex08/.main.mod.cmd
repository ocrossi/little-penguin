savedcmd_/home/octo/little-penguin/ex08/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/home/octo/little-penguin/ex08/"$$0) }' > /home/octo/little-penguin/ex08/main.mod
