.. Lomiri App Launch documentation master file, created by
   sphinx-quickstart on Thu Apr  7 09:22:13 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :maxdepth: 2

Overview
========

Lomiri App Launch is the abstraction that creates a consistent interface for
managing apps on Lomiri Touch. It is used by Lomiri and other programs to
start and stop applications, as well as query which ones are currently open.
It doesn't have its own service or processes though, it relies on the system
init daemon to manage the processes (currently systemd_) but configures them
in a way that they're discoverable and usable by higher level applications.

.. _systemd: http://freedesktop.org/wiki/Software/systemd/


Environment Variables
=====================

There are a few environment variables that can effect the behavior of UAL while
it is running.

LOMIRI_APP_LAUNCH_DEMANGLER
  Path to the UAL demangler tool that will get the Mir FD for trusted prompt session.

LOMIRI_APP_LAUNCH_DISABLE_SNAPD_TIMEOUT
  Wait as long as Snapd wants to return data instead of erroring after 100ms.

LOMIRI_APP_LAUNCH_LEGACY_ROOT
  Set the path that represents the root for legacy applications.

LOMIRI_APP_LAUNCH_LIBERTINE_LAUNCH
  Path to the libertine launch utility for setting up libertine applications.

LOMIRI_APP_LAUNCH_OOM_HELPER
  Path to the setuid helper that configures OOM values on application processes that we otherwise couldn't, mostly this is for Oxide.

LOMIRI_APP_LAUNCH_OOM_PROC_PATH
  Path to look for the files to set OOM values, defaults to `/proc`.

LOMIRI_APP_LAUNCH_SNAP_BASEDIR
  The place where snaps are installed in the system, `/snap` is the default.

LOMIRI_APP_LAUNCH_SNAP_LEGACY_EXEC
  A snappy command that is used to launch legacy applications in the current snap, to ensure the environment gets configured correctly, defaults to `/snap/bin/lomiri-session.legacy-exec`

LOMIRI_APP_LAUNCH_SNAPD_SOCKET
  Path to the snapd socket.

LOMIRI_APP_LAUNCH_SYSTEMD_CGROUP_ROOT
  Path to the root of the cgroups that we should look in for PIDs. Defaults to `/sys/fs/cgroup/systemd/`.

LOMIRI_APP_LAUNCH_SYSTEMD_PATH
  Path to the dbus bus that is used to talk to systemd. This allows us to talk to the user bus while Upstart is still setting up a session bus. Defaults to `/run/user/$uid/bus`.

LOMIRI_APP_LAUNCH_SYSTEMD_NO_RESET
  Don't reset the job after it fails. This makes it so it can't be run again, but leaves debugging information around for investigation.



API Documentation
=================

AppID
-----

.. doxygenstruct:: lomiri::app_launch::AppID
   :project: liblomiri-app-launch
   :members:
   :undoc-members:

Application
-----------

.. doxygenclass:: lomiri::app_launch::Application
   :project: liblomiri-app-launch
   :members:
   :undoc-members:

Helper
------

.. doxygenclass:: lomiri::app_launch::Helper
   :project: liblomiri-app-launch
   :members:
   :undoc-members:

Registry
--------

.. doxygenclass:: lomiri::app_launch::Registry
   :project: liblomiri-app-launch
   :members:
   :undoc-members:

Implementation Details
======================

Application Implementation Base
-------------------------------

.. doxygenclass:: lomiri::app_launch::app_impls::Base
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Implementation Legacy
---------------------------------

.. doxygenclass:: lomiri::app_launch::app_impls::Legacy
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Implementation Libertine
------------------------------------

.. doxygenclass:: lomiri::app_launch::app_impls::Libertine
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Implementation Snappy
---------------------------------

.. doxygenclass:: lomiri::app_launch::app_impls::Snap
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Info Desktop
------------------------

.. doxygenclass:: lomiri::app_launch::app_info::Desktop
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Info Snap
------------------------

.. doxygenclass:: lomiri::app_launch::app_impls::SnapInfo
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Icon Finder
------------------------

.. doxygenclass:: lomiri::app_launch::IconFinder
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Storage Base
------------------------

.. doxygenclass:: lomiri::app_launch::app_store::Base
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Storage Legacy
--------------------------

.. doxygenclass:: lomiri::app_launch::app_store::Legacy
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Storage Libertine
-----------------------------

.. doxygenclass:: lomiri::app_launch::app_store::Libertine
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Application Storage Snap
------------------------

.. doxygenclass:: lomiri::app_launch::app_store::Snap
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Helper Implementation Base
--------------------------

.. doxygenclass:: lomiri::app_launch::helper_impls::Base
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Jobs Manager Base
-----------------

.. doxygenclass:: lomiri::app_launch::jobs::manager::Base
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Jobs Instance Base
------------------

.. doxygenclass:: lomiri::app_launch::jobs::instance::Base
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Registry Implementation
-----------------------

.. doxygenclass:: lomiri::app_launch::Registry::Impl
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Snapd Info
----------

.. doxygenclass:: lomiri::app_launch::snapd::Info
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Type Tagger
-----------

.. doxygenclass:: lomiri::app_launch::TypeTagger
   :project: liblomiri-app-launch
   :members:
   :protected-members:
   :private-members:
   :undoc-members:

Quality
=======

Merge Requirements
------------------

This documents the expections that the project has on what both submitters
and reviewers should ensure that they've done for a merge into the project.

Submitter Responsibilities
..........................

	* Ensure the project compiles and the test suite executes without error
	* Ensure that non-obvious code has comments explaining it

Reviewer Responsibilities
.........................

	* Did the Jenkins build compile?  Pass?  Run unit tests successfully?
	* Are there appropriate tests to cover any new functionality?
	* If this MR effects application startup:
		* Run test case: lomiri-app-launch/click-app
		* Run test case: lomiri-app-launch/legacy-app
		* Run test case: lomiri-app-launch/secondary-activation
	* If this MR effect untrusted-helpers:
		* Run test case: lomiri-app-launch/helper-run
