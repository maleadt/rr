Done
----

Removed blacklist from https://github.com/mozilla/rr/pull/489

Added support for initial `ioctls`, harvested from:
- https://github.com/envytools/envytools/blob/master/demmt/nvrm_ioctl.h
- https://nick-black.com/dankwiki/index.php/Libcudest
- https://github.com/shinpei0208/gdev (re-use ioctls from demtt)
This now allows to record traces.

I've also added (untested) a special-case `ioctl` handling case for the CREATE one which
writes back to a pointer. Useful to develop this are the actual use cases from the `gdev`
NVRM back-end, rather than just looking at the `ioctl` argument fields.


Tools
-----

- https://nouveau.freedesktop.org/wiki/Valgrind-mmt/
- demmt from https://github.com/envytools/envytools/tree/master/demmt



Status
------

At time=346 a nvrm_ioctl_call causes memory divergence at a seemingly unrelated stack address...
(even though before that another nvrm_ioctl_call succeeds? check this)


ToDo
----

UVM ioctls are pretty messy (eg. type 0, nr 1) so we better match on the fd as well