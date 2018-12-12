FOR /r E:\Work\iSpeak\_server\src\audio_memdb\final_release_multi_logic\client_protocol_handlers\handlers\ %%G IN (*.cpp;*.hpp) DO (
	E:\Work\iSpeak\tools\CodeReviewAndCodeStyle\clang-format\clang-format.exe -i --style=file %%G
)
pause