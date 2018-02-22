kdbxmerge
=========

I've not found an option yet in keepass clients
to merge two keepass databases, .kdbx files.

This tool, once it's finished, will aim to 
do that. I'm not sure if merging databases in 
general is actually a good idea, but 
anyway, the Goals:

* kdbxmerge a.kdbx b.kdbx c.kdbx target.kdbx
  
  this would create a new target.kdbx file.
  
  To do this, it would need to ask the password
  of the existing kdbx files, and twice for
  a password for the new one.
* Also support .xml exports from the windows
  keepass client. And being abe to write these
  .xml files. I want to support this, because
  I dont want myself or users to be dependent
  on the crypto part of this program. Maybe it
  has bugs, or something.
* Be able to specify the password on the 
  commandline. I know it's insecure becuause
  it ends up in your bash history file.
