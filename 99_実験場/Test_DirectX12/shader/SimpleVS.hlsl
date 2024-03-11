//==========================================
//
//  ���_�V�F�[�_�[(SimpleVS.hlsl)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  VSInput�\���̒�`
//==========================================
struct VSInput
{
    float3 m_pos : POSITION; // �ʒu���W
    float4 m_col : COLOR; // ���_�J���[
};

//==========================================
//  VSOutput�\���̒�`
//==========================================
struct VSOutput
{
    float4 m_pos : SV_Position; // �ʒu���W
    float4 m_col : COLOR; // ���_�J���[
};

//==========================================
//  Transform�萔�o�b�t�@��ǉ�
//==========================================
cbuffer Transform : register(b0)
{
    float4x4 World : packoffset(c0); // ���[���h�s��
    float4x4 View : packoffset(c4); // �r���[�s��
    float4x4 Proj : packoffset(c8); // �ˉe�s��
}

//==========================================
//  ���_�V�F�[�_�̃��C���G���g���[�|�C���g
//==========================================
VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    float4 localPos = float4(input.m_pos, 1.0f);
    float4 worldPos = (World, localPos);
    float4 viewPos = mul(View, worldPos);
    float4 projPos = mul(Proj, viewPos);

    output.m_pos = projPos;
    output.m_col = input.m_col;
    
    return output;
}
