
#(c) Tim Zaman 2012
 
use warnings;
use strict;
 
use Win32::OLE qw(in);
 
my($input) = $ARGV[0];
my $exitcode;
 
my $objWMI = Win32::OLE->GetObject('winmgmts://./root/cimv2');
my $procs = $objWMI->InstancesOf('Win32_Process');
 
my @hits;
foreach my $p (in $procs) {
#push @hits => [ $p->Name, $p->ProcessID ]
    if($p->Name eq $input){
        my($pidnow)=int($p->ProcessID);
        my($procnow)=$p->Name;
        #Now we kill the process
        kill 9, $pidnow; #SIGQUIT (quit signal) = 9
        print "Quit process: ", $procnow, ' (', $pidnow ,")n";
    }
}