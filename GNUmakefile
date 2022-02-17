RC = windres

dbaurstart.exe: dbaurshim.o version.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

version.o: version.rc
	$(RC) $< $@

clean:
	rm -f dbaurstart.exe dbaurshim.o version.o version.res

.PHONY: clean
