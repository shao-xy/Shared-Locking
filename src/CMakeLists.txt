# Add the executable
add_executable(sl
	shared_locking.c
	args.c
	lock.c
	subcommand.c
)

# Set the install target
install(TARGETS sl
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ SETUID
        DESTINATION /usr/local/bin)
