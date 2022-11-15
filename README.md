
## VoodooPS2FocalTech

Modified kernel extension that enables focaltech ps2 touchpads (ACPI name FTL0101,FTL0102,FTL0103).
Works over VoodooI2C MT2 Engine All the gestures and buttons should work.
Note:Fn Key to disable touchpad doesn't work even with ACPI Patch.
## Installation
* Download [VoodooPS2Controller](https://github.com/acidanthera/VoodooPS2/releases) (v2.2.5 or above)
* Disable VoodooPS2Mouse, VoodooPS2Trackpad PlugIns in `/EFI/OC/config.plist`
* Copy VoodooPS2FocalTech to /`EFI/OC/Kexts/`
* Add VoodooPS2FocalTech in `config.plist under Kernel -> Add` after VoodooPS2Controller entry
* Save config.plist and Reboot
  

## Credits
* chilledHamza - [Linux port of Focaltech Driver for FTE0001] https://github.com/chilledHamza/VoodooPS2FocalTech
* Seth Forshee – [Touchpad Protocol Reverse Engineering](http://www.forshee.me/2011/11/18/touchpad-protocol-reverse-engineering.html)
* Alexandred, ben9923 & others – [VoodooI2C](https://github.com/alexandred/VoodooI2C)
* RehabMan, turbo, mackerintel, nhand42, phb, Chunnan, jape, bumby, Dense, acidanthera, kprinssu & others – [VoodooPS2Contoller](https://github.com/acidanthera/VoodooPS2)
