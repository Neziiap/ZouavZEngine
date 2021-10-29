pipeline
{
    agent any
	parameters
    {
        choice(name: 'Configuration', choices: ['Release','Debug'], description: 'Client Configuration')
		choice(name: 'Platform', choices: ['x64', 'Win32'], description: 'Platform architecture')
    }
	environment
	{
		configFilePath = "Config.json"
		MSBuildPath = "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe\""
	}
	stages
	{
		stage("Version")
		{
			steps
			{
				script
				{
					String branch = env.BRANCH_NAME
					if(branch != null && branch.contains("release"))
					{
						echo "Update version with branch name"
						def versions = (branch =~ /\d+/).findAll()						
						env.VERSION = versions.join('.')
						
						def configFile = readJSON file: "${env.configFilePath}"
						configFile.Project.Version = versions as String
						writeJSON(file: "${env.configFilePath}", json: configFile, pretty: 4)
					}
					else
					{
						echo "Update version number with file"
						
						def configFile = readJSON file: "${env.configFilePath}"
						env.VERSION = (configFile.Project.Version as String[]).join('.');
					}
					echo env.VERSION
				}
			}			
		}
		stage("Build")
		{
			steps
			{
				bat "${env.MSBuildPath} \"${WORKSPACE}\\ZouavZEngine.sln\" /t:ZouavZEngine /p:Configuration=${params.Configuration} /p:Platform=${params.Platform} "
			}
			post
    		{
    			success
    			{
    			    script
    			    {
    			        def configFile = readJSON file: "${env.configFilePath}"
    					for(String fileName in configFile.Project.Dependencies) 
    					{
    					    
    						def file = findFiles(glob: "**/${fileName}")
    						echo "${file}"
    						bat("copy /Y \"${file[0].path}\" \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\"")
    					}
    					for(String dirName in configFile.Project.ResourcesDir) 
    					{
    						echo "${dirName}"
    						bat("move /Y ${dirName} \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\"")
    					}
						echo "Zip contains"
						bat "dir \"${WORKSPACE}\\${params.Platform}\\${params.Configuration}\""
						
    					bat "dir"
						
    					env.Name = "${configFile.Project.Name}"
						
    					def exists = fileExists "${env.Name}_${env.VERSION}.zip"
    					if(exists)
    					    bat "del \"${env.Name}_${env.VERSION}.zip\""
    					
    					zip archive: true, dir: "${WORKSPACE}\\${params.Platform}\\${params.Configuration}", zipFile: "${env.Name}_${env.VERSION}.zip"
						archiveArtifacts artifacts: "${env.Name}_${env.VERSION}.zip"
    			    }
    			}
    		}
		}
		
		stage('Documentation')
		{
	    	when 
    		{
				anyOf
				{
					branch "release*";
					branch "master"
					branch "dev"
				}
    		}
		    stages
		    {
        		stage('MarkDown')
            	{
                    steps
                    {
                        script
                        {
                            def configFile = readJSON file: "${env.configFilePath}"
                            for (def file in configFile.Project.MarkDown) 
                            {
            					echo "${file}"
            					pwsh """\$md = ConvertFrom-Markdown -Path \"${file.File}\"
            					\$md.Html | Out-File -Encoding ${file.Encoding} ./${file.HTMLName}"""
            					
            					archiveArtifacts artifacts: "${file.HTMLName}", onlyIfSuccessful: true
            				}
                        }
            	    }
            	}
            	stage('Doxygen')
            	{
            	    steps
            	    {
            	        bat "doxygen Doxyfile"
						zip archive: true, dir: "${WORKSPACE}\\html", zipFile: "DoxygenHTML.zip"
						archiveArtifacts artifacts: "DoxygenHTML.zip", onlyIfSuccessful: true
            	    }
            	}
		    }
		}
	}
}