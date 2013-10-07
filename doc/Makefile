html: CHANGES.html INSTALL.html README.html index.html reference.html

%.html: %.mdwn pandoc.html
	pandoc --template=pandoc.html --css=pandoc.css --toc -o $@ $<

index.html: index.mdwn pandoc.html
	pandoc --template=pandoc.html --css=pandoc.css -V pagetitle= -o $@ $<

pandoc.html: contents.mdwn pandoc.html.in
	pandoc --template=pandoc.html.in -o $@ $<

%.png: %.ps
	convert -resize 128x128 -strip $< $@
	optipng $@

clean:
	$(RM) *.html

.INTERMEDIATE: pandoc.html
