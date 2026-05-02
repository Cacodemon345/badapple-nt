Bad Apple on NT
===============

PortAudio files compiled from commit `375345a752822e795bf6daeaa8e9ba981389c0ca`.

(See demo here: https://www.youtube.com/watch?v=5-iIKeebWzE)


Instructions
============
1. Download Microsoft Validation OS.
2. Follow instructions in https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/validation-os-customize-advanced?view=windows-11#mount-your-validation-os-image-1 to apply the WIM file (ValidationOS.wim) to one of the partitions on your PC (or your USB drive if you somehow did manage to create a bootable USB drive with at least a FAT32 partition and NTFS but it must only be applied to NTFS ones; look in https://ss64.com/nt/bcdboot.html for details but run this command on the FAT32 partition).
3. Use https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/dism-operating-system-package-servicing-command-line-options?view=windows-11#add-package to add `Microsoft-WinVOS-Audio-Package` (and all necessary prerequisite packages) beforehand (as specified in https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/validation-os-optional-packages?view=windows-11).
4. Make sure there's a boot entry for it to boot into it; you can use Visual BCD Editor to add entries for it if it's on the PC partitions.
5. Boot into it.
6. Install drivers from https://github.com/pal1000/Realtek-UAD-generic/ releases with `pnputil` like so (https://github.com/hobbit19/realtek-universal-audio-driver/#b-using-pnputil-only-for-user-who-have-hardware-id-is-not-found-error-when-using-universal-audio-driver-inf-editor, also, mind the `XXX` markings!).
7. Once you're done installing all of these, run `sndvol.exe` from your main Windows installation in the Validation OS command prompt. It should be able to detect audio devices at the very least.
8. Reboot back to main Windows. Remove `Microsoft-WinVOS-TerminalServices-Package` package (https://learn.microsoft.com/en-us/windows-hardware/manufacture/desktop/dism-operating-system-package-servicing-command-line-options?view=windows-11#remove-package) and add Win0N-RL2.Package.cab.
9. Reboot into text mode, and run the demo.
