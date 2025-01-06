savedcmd_/home/octo/little-penguin/ex07/debugfs.mod := printf '%s\n'   debugfs.o | awk '!x[$$0]++ { print("/home/octo/little-penguin/ex07/"$$0) }' > /home/octo/little-penguin/ex07/debugfs.mod
