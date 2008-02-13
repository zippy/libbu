" Vim filetype plugin file
" Language: Taf

" Only do this when not done yet for this buffer
if exists("b:did_ftplugin")
  finish
endif

" Don't load another plugin for this buffer
let b:did_ftplugin = 1

" Set format options -- allow comment formatting with gq, but disable
" other processing
setlocal fo-=tcrowan2 fo+=q

" Set 'comments' to be the same as C/C++
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://

" Win32 can filter files in the browse dialog
if has("gui_win32") && !exists("b:browsefilter")
  let b:browsefilter = "Textual Archive Format (*.taf)\t*.taf\n" .
	\ "All Files (*.*)\t*.*\n"
endif
