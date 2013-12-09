#
# spec file for package yast2-perl-bindings
#
# Copyright (c) 2013 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#


Name:           yast2-perl-bindings
Version:        3.1.1
Release:        0

BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source0:        %{name}-%{version}.tar.bz2

Group:	        System/YaST
License:        GPL-2.0+
BuildRequires:	gcc-c++ yast2-core-devel yast2-ycp-ui-bindings-devel libtool
BuildRequires:  yast2-devtools >= 3.0.6
BuildRequires:  yast2-ruby-bindings >= 1.0.0

# YCPValue::valuetype_str()
Requires:	yast2-core >= 2.16.37
BuildRequires:  yast2-ycp-ui-bindings-devel >= 2.16.37
Requires:       yast2-ycp-ui-bindings       >= 2.16.37
Requires:	perl = %{perl_version}
%if 0%{?suse_version} < 1220
BuildRequires:  libxcrypt-devel
%endif
# for YaPI.pm
Requires:	perl(Locale::gettext)

Summary:	YaST2 - Perl Bindings

%description
This adds an embedded Perl interpreter to YaST2 as a plug-in (in other
words it will be loaded only if required). This is a very efficient way
of calling Perl from within YaST2 YCP scripts.

%prep
%setup -n %{name}-%{version}

%build
%yast_build

%install
%yast_install

rm $RPM_BUILD_ROOT/%{yast_plugindir}/libpy2lang_perl.la
rm $RPM_BUILD_ROOT/%{perl_vendorarch}/auto/YaST/YCP/libYCP.la


%files
%defattr (-, root, root)
%{yast_plugindir}/libpy2lang_perl.so.*
%{yast_plugindir}/libpy2lang_perl.so
# libYCP goes elsewhere
%dir %{perl_vendorarch}/YaST
%{perl_vendorarch}/YaST/YCP.pm
%dir %{perl_vendorarch}/auto/YaST
%dir %{perl_vendorarch}/auto/YaST/YCP
%{perl_vendorarch}/auto/YaST/YCP/libYCP.so*
%dir %{yast_moduledir}
%{yast_moduledir}/YaPI.pm
%{yast_ydatadir}/devtools/pluglib-bindings
%{yast_ydatadir}/devtools/admin/aminclude/pluglib-bindings.ami
%doc %{yast_docdir}
