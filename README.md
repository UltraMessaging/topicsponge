# topicsponge

Example code to log discovered sources and their source strings.

# Table of contents

<!-- mdtoc-start -->
&bull; [topicsponge](#topicsponge)  
&bull; [Table of contents](#table-of-contents)  
&bull; [Copyright And License](#copyright-and-license)  
&bull; [Repository](#repository)  
&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Configuring Topicsponge](#configuring-topicsponge)  
<!-- TOC created by '/home/sford/bin/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->

# Copyright And License

All of the documentation and software included in this and any
other Informatica Ultra Messaging GitHub repository
Copyright (C) Informatica, 2024. All rights reserved.

Permission is granted to licensees to use
or alter this software for any purpose, including commercial applications,
according to the terms laid out in the Software License Agreement.

This source code example is provided by Informatica for educational
and evaluation purposes only.

THE SOFTWARE IS PROVIDED "AS IS" AND INFORMATICA DISCLAIMS ALL WARRANTIES
EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF
NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
PURPOSE.  INFORMATICA DOES NOT WARRANT THAT USE OF THE SOFTWARE WILL BE
UNINTERRUPTED OR ERROR-FREE.  INFORMATICA SHALL NOT, UNDER ANY CIRCUMSTANCES,
BE LIABLE TO LICENSEE FOR LOST PROFITS, CONSEQUENTIAL, INCIDENTAL, SPECIAL OR
INDIRECT DAMAGES ARISING OUT OF OR RELATED TO THIS AGREEMENT OR THE
TRANSACTIONS CONTEMPLATED HEREUNDER, EVEN IF INFORMATICA HAS BEEN APPRISED OF
THE LIKELIHOOD OF SUCH DAMAGES.

# Repository

See https://github.com/UltraMessaging/topicsponge for code and documentation.

# Introduction

It can be useful keep track of which publishers create which sources.
For publishers that create many sources, it can be useful to track which
topics are mapped to which transport sessions.

The topicsponge program discovers UM sources and prints
the topic and source string to standard out.
It uses UM's
[resolver_source_notification_function](https://ultramessaging.github.io/currdoc/doc/Config/grpmajoroptions.html#resolversourcenotificationfunctioncontext)
callback to discover sources via topic resolution source advertisements.

The topicsponge program is intentionally written very simply to make it
easy to be customized for a user's environment.

Depending on your applications' configurations,
UM can stop advertising sources after a time.
To get a complete list of sources,
you'll have to start the topicsponge before the publishers are started.
Ideally the topicsponge would be running continuously any time
applications are running.

Note that topicsponge does not detect when a source is deleted or
if the publisher exits.
It cannot monitor for EOS or detect timeouts.
However,
[automatic monitoring](https://ultramessaging.github.io/currdoc/doc/Operations/monitoring.html#automaticmonitoring)
can monitor the liveness of transport sessions.

In a network that has multiple
[topic resolution domains](https://ultramessaging.github.io/currdoc/doc/Design/fundamentalconcepts.html#topicresolutiondomain)
connected via DROs,
the topicsponge will only reliably detect sources that originate in the same
TRD as the topicsponge.
The topicsponge can detect proxy sources creeated by the DRO on behalf of
remote publishers, but only for topics that the DRO creates to satisfy
subscribers.
To get a complete picture of a UM network's sources,
you should run a topicsponge in every TRD in the network.

In a TRD that is serviced by an SRS,
the topicsponge should be configured with
[esolver_service_interest_mode (context)](https://ultramessaging.github.io/currdoc/doc/Config/grptcpbasedresolveroperation.html#resolverserviceinterestmodecontext)
set to "flood".

## Configuring Topicsponge

In the interest of keeping the topicsponge's code simple,
it does not load a configuration file.
Instead, it relies on environment variables to specify a configuration file.

For example, for an
[XML-based config file](https://ultramessaging.github.io/currdoc/doc/Config/xmlconfigurationfiles.html):
````
export LBM_XML_CONFIG_APPNAME="topicsponge"
export LBM_XML_CONFIG_FILENAME="topicsponge.xml"
Linux64/topicsponge >topicsponge.log
````

Or for a ["flat" config file](https://ultramessaging.github.io/currdoc/doc/Config/plaintextconfigurationfiles.html):
````
export LBM_DEFAULT_CONFIG_FILE="topicsponge.cfg"
Linux64/topicsponge >topicsponge.log
````
