ux='iamok'
uy='iamok'
uz=''
if [ "$ux" = "$uy" ]
then
    echo 'ux is equal to uy'
fi
if [ "$ux" != "$uy" ]
then
    echo 'ux is not equal to uy'
fi
[ -z "$uz" ] && echo 'uz is null'
[ -z "$ux" ] || echo 'ux is not null'
