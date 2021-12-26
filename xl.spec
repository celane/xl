%define NVdir %{name}-%{version}

Name:           xl
Version:        1.0
Release:        1%{?dist}
Summary:        super-simple X screen locker

License:        BSD 3-Clause
URL:            https://github.com/celane/xl

Requires:       gcc
Requires:       make
Requires:       pam
BuildRequires:  pam-devel

%description
Super simple X screen locker

%prep
rm -rf %{NVdir}
git clone %{url}.git %{NVdir}

%build
cd %{NVdir}
make

%install
pushd %{NVdir}
install -d %{buildroot}%{_bindir}
install xl %{buildroot}%{_bindir}
popd

%files
%license %{NVdir}/LICENSE
%doc %{NVdir}/README.md 
%{_bindir}/xl

%changelog
* Wed Dec 22 2021 Chuck Lane <lane@duphy4.physics.drexel.edu> - 1.0-1
- initial spec file

