TEX_SRCS := $(wildcard ./*.tex)


all : report.pdf


report.pdf : $(TEX_SRCS)
	pdflatex report.tex
	pdflatex report.tex
	if command -v gs > /dev/null; then \
            gs -sDEVICE=pdfwrite \
            -dCompatibilityLevel=1.4 \
            -dNOPAUSE \
            -dOptimize=true \
            -dQUIET \
            -dBATCH \
            -dRemoveUnusedFonts=true \
            -dRemoveUnusedImages=true \
            -dOptimizeResources=true \
            -dDetectDuplicateImages \
            -dCompressFonts=true \
            -dEmbedAllFonts=true \
            -dSubsetFonts=true \
            -dPreserveAnnots=true \
            -dPreserveMarkedContent=true \
            -dPreserveOverprintSettings=true \
            -dPreserveHalftoneInfo=true \
            -dPreserveOPIComments=true \
            -dPreserveDeviceN=true \
            -dMaxInlineImageSize=0 \
            -sOutputFile="tmp.pdf" \
            "report.pdf" && \
            mv tmp.pdf $@; \
            fi

compress :
	for file in ./images/*.pdf; do \
            gs -sDEVICE=pdfwrite \
            -dCompatibilityLevel=1.4 \
            -dNOPAUSE \
            -dOptimize=true \
            -dQUIET \
            -dBATCH \
            -dRemoveUnusedFonts=true \
            -dRemoveUnusedImages=true \
            -dOptimizeResources=true \
            -dDetectDuplicateImages \
            -dCompressFonts=true \
            -dEmbedAllFonts=true \
            -dSubsetFonts=true \
            -dPreserveAnnots=true \
            -dPreserveMarkedContent=true \
            -dPreserveOverprintSettings=true \
            -dPreserveHalftoneInfo=true \
            -dPreserveOPIComments=true \
            -dPreserveDeviceN=true \
            -dMaxInlineImageSize=0 \
            -sOutputFile="tmp.pdf" \
            "$${file}" && \
            mv tmp.pdf "$${file}"; \
            done

clean :
	$(RM) ./*.aux ./*.bbl ./*.log ./*.out ./*.toc ./*.blg

.PHONY : clean compress all
