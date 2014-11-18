g++ test_pi0_gen.cc -o test_pi0_gen.exe `root-config --cflags` `root-config --libs` -I/sps/clas/rafopar/dvcs_gen -L/sps/clas/rafopar/dvcs_gen -lTGenPi0 -lTGenGeo -lTGenBase -I/afs/in2p3.fr/home/throng/clas/alex/soft/soft-2 -L/afs/in2p3.fr/home/throng/clas/alex/soft/soft-2 -lDVCS /afs/in2p3.fr/home/throng/clas/carlos/dvcs/soft/hrse_acceptance_maps2.o

