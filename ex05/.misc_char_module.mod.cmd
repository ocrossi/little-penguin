savedcmd_misc_char_module.mod := printf '%s\n'   misc_char_module.o | awk '!x[$$0]++ { print("./"$$0) }' > misc_char_module.mod
