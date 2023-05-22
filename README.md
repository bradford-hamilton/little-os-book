unfinished

###### Going through https://littleosbook.github.io
###### Other resources I have open:
###### - https://wiki.osdev.org
###### - http://www.jamesmolloy.co.uk

---

_Quick notes_

compile loader.s into a 32 bit ELF
```
nasm -f elf32 loader.s
```

link executable
```
ld -T link.ld -melf_i386 loader.o -o kernel.elf
```

generate iso image
```
genisoimage -R                    \
  -b boot/grub/stage2_eltorito    \
  -no-emul-boot                   \
  -boot-load-size 4               \
  -A os                           \
  -input-charset utf8             \
  -quiet                          \
  -boot-info-table                \
  -o os.iso                       \
  iso
```
