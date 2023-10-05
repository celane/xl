define %debug_package %{nil}

Name:           xl
Version:        %{version_}
Release:        2%{?dist}
Summary:        super-simple X screen locker

License:        BSD 3-Clause
URL:            https://github.com/celane/xl
Source0:        xl-%{version}.tar.gz

Requires:       pam
BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  libX11-devel
BuildRequires:  libxcrypt-devel
BuildRequires:  pam-devel

%description
Super simple X screen locker

%prep
%setup -q

%build
make

%install
install -d %{buildroot}%{_bindir}
install xl %{buildroot}%{_bindir}

%files
%license LICENSE
%doc README.md 
%{_bindir}/xl

%changelog
* Thu Sep 28 2023 Chuck Lane <lane@dchooz.org> - 1.0-2
- fix stuff needed for github autobuild
* Wed Dec 22 2021 Chuck Lane <lane@duphy4.physics.drexel.edu> - 1.0-1
- initial spec file

