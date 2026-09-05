# Bifrost

> [!Note]
> This README is intended as an introduction to Bifrost's first version up to completion. Since that's under development at this time, some of this file's claims may not match the current state of Bifrost.
- [What is Bifrost?](#what-is-bifrost)
- [What are Bifrost specifications?](#what-are-bifrost-specifications)
- [How do I get Bifrost?](#how-do-i-get-bifrost)
- [Why "Bifrost"?](#why-bifrost)
- [What is Bifrost's license?](#what-is-bifrosts-license)

## What is Bifrost?
Bifrost is an operating system built for learning purposes. It doesn't aim to be even remotely compared to a professional OS, or to have any distinctive trait or performance feature.

## What are Bifrost specifications?
Bifrost is based on a **monolithic kernel**, boots on **legacy BIOS** and targets the **x86_64 architecture**.

## How do I get Bifrost?
To get Bifrost, you first need to resolve some dependencies. To be precise, you'll need a cross-compiler **i686-elf-gcc**, a cross-linker **i686-elf-ld**, and **make**.

> [!Note]
> Also, I still haven't tried Bifrost outside of an emulated environment, and neither should you, so you will also need a system emulator **qemu-system-i386**.

Once all dependencies are resolved, you can clone the repository and build the OS by running
```
make
```
in the project folder.
> Or, if you're using QEMU, you can directly run it with
> ```
> make run
> ```

## Why "Bifrost"?
I loved the idea of discovering what's behind known operating systems. You get hardware paired with just some very essential code, plug a USB drive with an OS in, click install, and then you can do virtually anything (unless that game you'd like to play *has* specs and you bought a potato). The operating system does **a lot** by providing a "bridge" between the machine and the user so, since I decided to build one myself, I thought "Bifrost" would be a well suiting name for it. 

By the way, if you were wondering, that's the name of the bridge that reaches between the Earth and the realm of the gods in Norse mythology.

## What is Bifrost's license?
Bifrost is licensed under the GNU General Public License v3.0. For details, see [LICENSE](LICENSE).