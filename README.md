# Toxins

The Toxins repository is a collection of small utilities, mostly written in C
or C++, that use toxcore. The number of dependencies these tools have is very
small, so they don't need complicated build systems.

If you have a utility you'd like to see included here, feel free to send a
[Pull Request](https://github.com/TokTok/toxins/compare).

## Requirements

* The tool must be compiled with a **single compiler invocation**.
* The execution does not require much setup.
* It must be **linked statically** so that the resulting binary can be easily
  distributed.
* The number of source files should be small, ideally 1 or 2, but a larger
  number is acceptable as long as you just need a single compiler command to
  compile and link it.
* The tool must have a `--help` or `-h` flag that outputs at least a minimal
  usage line.

If the tool has a self-test flag, you may add it to the `check` target of the
tool's `Makefile`. By default, at least the `--help` flag is tested there.

## List of toxins

* **irc_syncbot**: Bot that synchronizes an IRC channel and a Tox group chat
  (conference).
* **tox_shell**: Proof of concept SSH-like server software using Tox. Testing
  program, not intended for actual use.
* **tox_sync**: Bittorrent-sync-like software using Tox. Syncs two directories
  together.

## External toxins

This repository also links to some external repositories with more than a few
source files. We link these using git submodules, because we think they are
super cool and can be considered part of the TokTok stack, but are externally
maintained. These may need one or two `-DSOMETHING=something` flags to the
compiler, but are still pretty easy to compile. They may also have a few more
dependencies. Static linking is still a strict requirement. If your toxin is
listed here, we guarantee that we won't make changes that break your tool and
its self-test without contacting you first.

* [**toxvpn**](https://github.com/cleverca22/toxvpn): A client / server for a
  Tox-based VPN. Run the server on one computer, copy the public key, and run
  the client (with that key) on another computer, and it should Just Work™.
