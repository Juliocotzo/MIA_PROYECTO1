MKdisk -size->10 -path->/home/julio/MIA/Disco1.dsk
fdisk -size->4000 -path->/home/julio/MIA/Disco1.dsk -name->Particion1
#fdisk -size->5000 -path->/home/julio/MIA/Disco1.dsk -name->Particion2
mount -path->/home/julio/MIA/Disco1.dsk -name->Particion1
#mount -path->/home/julio/MIA/Disco1.dsk -name->Particion2
mount
rep -id->vda1 -path->/home/julio/MIA/REPORTES/reporte1.png -name->mbr
rep -id->vda1 -path->/home/julio/MIA/REPORTES/disk.png -name->disk

#MKFS

mkfs -type->FuLL -id->vda1
#mkfs -type->fast -id->vda2

#LOGIN
login -usr->root -pwd->123 -id->vda1
#login -usr->"mi usuario" -pwd->"mi pwd" -id->vda1

#LOGOUT
#logout

Mkdir -P -path->/home/user/docs/usac