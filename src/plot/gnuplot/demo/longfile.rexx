/* */

-- list file with file name translations
f = "rename.lst"
-- list file with files to patch
p = "patch.lst"

-- where to create new files
prefix = "short"

-- list of files to patch

-- read translation file
trans.0 = 0
count = 0
do while lines(f) > 0
  parse value linein(f) with long short
  short = strip(short)
  if long \= "" then do
	count = count + 1
	trans.count.!long = strip(long)
	trans.count.!short = short
	if length(short) > 8+3+1 then say "Warning: name too long:" short 
  end
end
trans.0 = count

say "Found" count "file name changes."

-- Check order of array: avoid finding substrings of file names listed later
do i = 1 to trans.0
  do k = 1 to i - 1
    if pos(trans.k.!long, trans.i.!long) > 0 then do
	  say "ERROR:" trans.k.!long "MUST be listed before" trans.i.!long
	  exit
    end
    if pos(trans.i.!long, trans.k.!long) > 0 then do
	  say "Info:" trans.k.!long "should be listed before" trans.i.!long
    end
  end
end

-- Create destination directory
"mkdir short"

-- create list of demo files
count = 0
patch.0 = 0
do while lines(p) > 0
  l = linein(p)
  if l \= "" then do
	count = count + 1
	patch.count = strip(l)
  end
end
patch.0 = count

say "Found" count "files to patch."

"dir /b | rxqueue"
count = 0
do while queued() > 0
  parse pull file
  count = count + 1

  -- skip known files
  if file = f then iterate
  if file = p then iterate
  if file = "longfile.rexx" then iterate
  if file = "Makefile.am.in" then iterate
  if file = prefix then iterate
  if file = "html" then iterate
  if file = "plugin" then iterate
  if file = "games" then iterate

  -- lookup in translation table
  dorename = 0
  t = file
  do i = 1 to trans.0 
    if trans.i.!long = file then do 
		dorename = 1
		t = trans.i.!short
	end
  end

  -- lookup in patch table
  dopatch = 0
  do i = 1 to patch.0 
    if patch.i = file then do 
		dopatch = 1
	end
  end

  m = ""
  if dopatch then
    m = "Patch"
  else
    m = "Copy "
  if dorename then 
    say count m file "  ->  " t
  else do
    if length(file) > 12 then
		say "count" m file "-- TOO LONG!"
	else
		say count m file
  end
  
  if \dopatch then do
	-- simply copy the file
	"copy" file prefix || "\" || t
  end
  else do
    -- patch this file
	say "Patching" file "..."
	"del" prefix || "\" || t
	do while lines(file)
		l = linein(file)
		-- search for replacement strings
		found = 0
		do i = 1 to trans.0
			if pos(trans.i.!long, l) > 0 then do
			   found = 1
			   -- say l
			   leave
			end
		end
		if \found then do
		  call lineout prefix || "\" || t, l
		end
		else do
		  parse var l a (trans.i.!long) b
		  call lineout prefix || "\" || t, a || trans.i.!short || b
		end
	end
  end
end
