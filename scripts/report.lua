local template = require "resty.template"

function template_print_array(k, array)
	print("\t" .. k .." = {")
	for _, item in ipairs(array) do
		print("\t\t{")
		for k, v in pairs(item) do
			if (k == "FocalFieldNames" or k == "FocalFieldValues") then
				print("\t\t\t" .. k .. " = {")
				for _, v1 in ipairs(v) do
					print("\t\t\t\t" .. v1 .. ",")
				end
				print("\t\t\t}")
			else
				print("\t\t\t" .. k .. " = " .. v .. ",")
			end
		end
		print("\t\t},")
	end
	print("\t},")
end

function template_print(t)
	print("{")
	for k, v in pairs(t) do
		if ( k == "Users" or k == "Groups") then
			template_print_array(k, v)
		elseif (k == "Defects") then
			defects_print(v)
		elseif (k == "FieldNames") then
			print("\tFieldName = {")
			for _, name in ipairs(v) do
				print("\t\t" .. name .. ",")
			end
			print("\t},")
		else
			print("\t" .. k .. " = " .. v .. ",")
		end
	end
	print("}")
end

function fieldNames_print(t)
	print("{\n\tFieldNames = {")
	for _, fieldName in ipairs(t) do
		print()
	end
	print("\t}")
end

function defects_print(t)
	print("\tDefects = {")	
	for _, a1 in ipairs(t) do
		print("\t\t{")
		for k1, v1 in pairs(a1) do
			if ( k1 == "FieldValues" ) then
				print("\t\t\tFieldValues = {")
				for _, v2 in ipairs(v1) do
					print("\t\t\t\t" .. v2 .. ",")
				end
				print("\t\t\t},")
			else
				print("\t\t\t" .. k1 .. " = " .. v1 .. ",")
			end
		end
		print("\t\t},")
	end
	print("\t},")
end

function test_print(testType, t)
	if (testType == "/home/root/template.html") then
		template_print(t)
	end
end

function output(ifile, ofile, t)
	--test_print(ifile, t)

	--local view = template.new "view.html"
	local f = assert(io.open(ofile, "w"))
	-- view.message = "Hello, World!"
	--f.write(view:render())
	-- Using template.render

	f:write(template.render(ifile, t))
	-- f.write(template.render("view.html", { message = "Hello, World!" }))
	f:close()
end
