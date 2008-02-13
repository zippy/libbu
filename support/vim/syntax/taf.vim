" Vim syntax file
" Language:	Nango
" Maintainer:	None :)
" Last Change:	2006 Aug 24

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif


sy region Group matchgroup=Operator start=+{+ end=+}+ contains=NamePair,Value,Group,Title,Comment,Operator
sy region Title matchgroup=Operator start=+{+ end=+:+ keepend
sy region Title matchgroup=Operator start=+{[ \t]*"+ skip=+\\"+ end=+"[ \t]*:+ keepend

sy region Value start=+"+ skip=+\\"+ end=+"+
sy match Value +[^{}:=" \t\r\n]\++
sy match Name +[^{}:=" \t\r\n]\+[ \t\r\n]*=+ contains=Operator

sy match Operator '}'
sy match Operator '='

sy region Comment start=+//+ end=+$+
sy region Comment start=+/\*+ end=+\*/+
" sy region NamePair matchgroup=Name start=+[^{}:=" \t\r\n]\+[ \t\r\n]*=+ matchgroup=Value end=+[^{}:=" \t\r\n]\++
" sy region NamePair matchgroup=Name start=+[^{}:=" \t\r\n]\+[ \t\r\n]*=+ matchgroup=Value end=+[^{}:=" \t\r\n]\++


hi def link NamePair Normal
hi def link Name Identifier
hi def link Value Constant
hi def link Group Statement

