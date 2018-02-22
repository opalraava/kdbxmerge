kdbxmerge
=========

I've not found an option yet in keepass clients
to merge two keepass databases, .kdbx files.

This tool, once it's finished, will aim to 
do that. I'm not sure if merging databases in 
general is actually a good idea, but 
anyway, the Goals:

* kdbxmerge a.kdbx b.kdbx c.kdbx target.kdbx
  
  this would create a new target.kdbx file. *the 
  source files become root folders in the target.*
  
  To do this, it would need to ask the password
  of the existing kdbx files, and twice for
  a password for the new one.
* Also support .xml exports from the windows
  keepass client. And being abe to write these
  .xml files. I want to support this, because
  I dont want myself or users to be dependent
  on the crypto part of this program. Maybe it
  has bugs, or something. Maybe the keystrokes
  in the pasword asking phase get logged. On the 
  other hand, they are 'naked' xml files ready 
  for copying.
* Be able to specify the password on the 
  commandline. I know it's insecure becuause
  it ends up in your bash history file. But one
  can use it for automated testing, things like that.
* There are things like key files, but I have no idea
  if it's worth supporting that.
* kdbxmerge myfile.kdbx target.kdbx

  one inputfile MUST work
  
  the idea is to 'add' each argument on the list to the target
  structure, and if there is only one of such thing, it should
  still work. also its nice to convert to/from xml on the commandline.
