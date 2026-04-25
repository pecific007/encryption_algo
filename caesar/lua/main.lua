#!/usr/bin/env lua

if #arg ~= 3 then
	io.stderr:write("Usage: " .. arg[0] .. " <input> <output> <key>")
end

local function main()
	local filename = arg[1]
	local source, err = io.open(filename, "r")
	if not source then
		io.stderr:write("Couldn't open file." .. err .. "\n")
		os.exit(1)
	end
	local pt = source:read("*a")

	local key = tonumber(arg[3])
	if not key then
		io.stderr:write("The key must be an integer value")
		os.exit()
	end

	local enc = Encrypt(pt, key)
	print("Plaintext: " .. pt)
	print("Encrypted: " .. enc)

	local Out, err = io.open(arg[2], "w")
	if not Out then
		io.stderr:write(err .. "\n")
		os.exit(1)
	end
	Out:write(enc)
	Out:close()
end

function Encrypt(pt, key)
	local enc = {}
	local A, a = string.byte("A"), string.byte("a")

	for i = 1, #pt do
		local c = pt:sub(i, i)
		local byte = string.byte(c)
		if byte >= A and byte <= (A + 25) then
			enc[#enc + 1] = string.char(((byte - A + key) % 26) + A)
		elseif byte >= a and byte <= (a + 25) then
			enc[#enc + 1] = string.char(((byte - a + key) % 26) + a)
		else
			enc[#enc + 1] = c
		end
	end
	return table.concat(enc)
end

main()
