GTKDOCIZE=$(which gtkdocize 2>/dev/null)
if test -z "${GTKDOCIZE}"; then
    echo "You don't have gtk-doc installed, and thus won't be able to generate the documentation."
    rm -f gtk-doc.make
    cat > gtk-doc.make <<EOF
EXTRA_DIST =
CLEANFILES =
EOF
else
    gtkdocize || exit $?
fi
