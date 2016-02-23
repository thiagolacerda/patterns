require 'zip'

def extractZipFiles(dir)
    open('tdrive.txt', 'w') do |f|
        Dir.glob(dir.concat('*.zip')) do |zipFile|
            Zip::File.open(zipFile) { |zip| zip.each { |entry| f << entry.get_input_stream.read } }
        end
    end
end

puts "arg: #{ARGV[0]}"
extractZipFiles(ARGV[0].dup)
